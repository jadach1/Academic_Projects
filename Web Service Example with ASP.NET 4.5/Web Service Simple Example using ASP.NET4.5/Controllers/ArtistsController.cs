using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class ArtistsController : Controller
    {

        Manager m = new Manager();

        // GET: Artists
        [Authorize]
        public ActionResult Index()
        {
            var o = m.ArtistGetAll();
            return View(o);
        }

        // GET: Artists/Details/5
        [Authorize]
        public ActionResult Details(int? id)
        {
            var o = m.ArtistGetOneWithMedia(id.GetValueOrDefault());
            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            {
                return View(o);
            }             
        }

        // GET: Artists/Create
        [Authorize(Roles = "Executive")]
        public ActionResult Create()
        {
            var form = new AddArtistForm();

           // form.BirthOrStartDate = DateTime.Now;

            // get list
            var o = m.GenreGetAll();

            if (o != null)
            {    // create list
                form.GenreList = new SelectList(o, "Id", "Name");
            }

            return View(form);
        }

        // POST: Artists/Create
        [HttpPost, ValidateInput(false)]
        [Authorize(Roles = "Executive")]
        public ActionResult Create(ArtistAdd newItem)
        {
         if (!ModelState.IsValid)
            {
                return View("Create");
            }
            var AddedItem = m.CreateArtist(newItem);
            if (AddedItem == null)
            {
                return View("Index");
            }                    
            else
            {
                return RedirectToAction("Details", new { id = AddedItem.Id });
            }
        }

        //GET: Artists/AddAlbumToArtist/{id}
        [Authorize(Roles = "Coordinator")]
        [Route("artist/{id}/AddAlbumToArtist")]
        public ActionResult AddAlbumToArtist(int? id)
        {
            var ArtistDetail = m.ArtistGetOne(id.GetValueOrDefault());

            if (ArtistDetail == null)
            {
                return HttpNotFound();
            }

            // create form
            var form = new AlbumAddForm();

            // copy artist name and id
            form.ArtistName = ArtistDetail.Name;
            form.ArtistId = ArtistDetail.Id;

            // create genre list
            form.GenreList = new SelectList(m.GenreGetAll(), "Id", "Name");

            return View(form);
        }

        //POST: Artists/AddAlbumTOArtist/{id}
        [HttpPost, ValidateInput(false)]
        [Authorize(Roles = "Coordinator")]
        [Route("artist/{id}/AddAlbumToArtist")]
        public ActionResult AddAlbumToArtist(AlbumAdd newItem)
        {
            if (!ModelState.IsValid)
            {
                var errors = ModelState.SelectMany(x => x.Value.Errors.Select(z => z.Exception));
                return View(newItem);
            }
            var AddedItem = m.AddAlbum(newItem);

            if (AddedItem == null)
            {
                return View("index");
            }
            else
            {
                return RedirectToAction("details", "albums", new { id = AddedItem.Id });
            }
        }
        // GET Artists/AddArtistMedia/{id}

        [Authorize(Roles = "Executive")]
        public ActionResult AddArtistMedia(int? id)
        {
            var o = m.ArtistGetOne(id.GetValueOrDefault());

            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            {
                var form = new ArtistMediaAddForm();

                // add associated data artist id and details
                form.ArtistId = o.Id;
                form.ArtistProperty = o.Name;

                return View(form);
            }
        }
        // POST Artists/AddArtistMedia/
        [HttpPost]
        [Authorize(Roles = "Executive")]
        public ActionResult AddArtistMedia(ArtistMediaAdd newItem)
        {
            if (!ModelState.IsValid)
            {
                var errors = ModelState.SelectMany(x => x.Value.Errors.Select(z => z.Exception));
                return View(newItem);
            }
            var AddedItem = m.AddArtistMedia(newItem);
            if ( AddedItem == null )
            {
                return View("index");
            }
            else
            {
                return RedirectToAction("details", new { id = AddedItem.Id });
            }
        }

        /*   [Authorize(Roles ="Amin,Coordinator")]
           public ActionResult RemoveAllMedia(int? id)
           {
               var o = m.ArtistGetOneWithMedia(id.GetValueOrDefault());
               if ( o != null )
               {
                       // call manager
                   m.RemoveArtistMedia(o);    
               }

               // return back to original details view
               return RedirectToAction("details", id.GetValueOrDefault());
           }*/

        // GET: Artists/Delete/5
        [Authorize(Roles = "Executive")]
        public ActionResult Delete(int? id)
        {
            var o = m.ArtistGetOne(id.GetValueOrDefault());
            if ( o == null)
            {
                return HttpNotFound();
            }
            else
            {
                return View(o);
            }
           
        }

        // POST: Artists/Delete/5
        [HttpPost]
        [Authorize(Roles = "Executive")]
        public ActionResult Delete(int? id, ArtistBase Item)
        {
            var ItemToDelete = m.DeleteArtist(id.GetValueOrDefault());

            return RedirectToAction("index");

        }
    }
}
