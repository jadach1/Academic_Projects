using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class TracksController : Controller
    {

        Manager m = new Manager();

        // GET: Tracks
        [Authorize]
        public ActionResult Index()
        {
            var o = m.TrackGetAllWithDetail();
      

            return View(o);
        }

        // GET: Tracks/Details/5
        [Authorize]
        public ActionResult Details(int? id)
        {
            var o = m.TrackGetOne(id.GetValueOrDefault());

            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            return View(o);
        }

        [Route("Clip/{id}"),Authorize]
        public ActionResult GetSampleClip(int? id)
        {
            var o = m.SampleClipById(id.GetValueOrDefault());

            if ( o == null )
            {
                return null;
            }
            else
            {
                // check to make sure media and mediacontent are not null
                if (o.Media == null && o.MediaContentType == null)
                {
                    return null;
                }
                if (o.MediaContentType == null && o.Media != null)
                {
                    o.MediaContentType = "default empty";
                }

                return File(o.Media, o.MediaContentType);
            }
        }
        // GET: Tracks/Create
        [Authorize(Roles ="Clerk")]
        public ActionResult Create()
        {
            return View();
        }

        // POST: Tracks/Create
        [HttpPost, Authorize(Roles = "Clerk")]
        public ActionResult Create(FormCollection collection)
        {
            try
            {
                // TODO: Add insert logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        // GET: Tracks/Edit/5
        public ActionResult Edit(int id)
        {
            return View();
        }

        // POST: Tracks/Edit/5
        [HttpPost]
        public ActionResult Edit(int id, FormCollection collection)
        {
            try
            {
                // TODO: Add update logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        // GET: Tracks/Delete/5
        [Authorize(Roles = "Clerk")]
        public ActionResult Delete(int id)
        {
            return View();
        }

        // POST: Tracks/Delete/5
        [HttpPost, Authorize(Roles = "Clerk")]
        public ActionResult Delete(int id, FormCollection collection)
        {
            try
            {
                // TODO: Add delete logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }
    }
}
