using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class ArtistMediaController : Controller
    {
        Manager m = new Manager();
        // GET: ArtistMedia
        public ActionResult Index()
        {
            return View();
        }

        // GET: ArtistMedia/Details/5
        [Route("media/{StringId}")][Authorize]
        public ActionResult Details(string stringId= "")
        {
            var o = m.ArtistMediaGetByString(stringId);

            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            {
                return File(o.Content, o.ContentType);
            }
            
        }

      /*  [Route("media/{stringId}/IconForDocument")]
        public ActionResult IconForDocument(string stringId = "")
        {
            // Attempt to get the matching object
            //var o = m.IconGetByString(stringId);

            if ( o == null)
            {
                return null;
            }
            else
            {
                return File(o.Content, o.ContentType);
            }
        }*/

        [Route("media/{stringId}/download")][Authorize]
        public ActionResult DetailsDownload(string stringId = "")
        {
            // Attempt to get the matching object
            var o = m.ArtistMediaGetByString(stringId);

            if (o == null)
            {
                return HttpNotFound();
            }
            else
            {
                // Get file extension, assumes the web server is Microsoft IIS based
                // Must get the extension from the Registry (which is a key-value storage structure for configuration settings, for the Windows operating system and apps that opt to use the Registry)

                // Working variables
                string extension;
                RegistryKey key;
                object value;

                // Open the Registry, attempt to locate the key
                key = Registry.ClassesRoot.OpenSubKey(@"MIME\Database\Content Type\" + o.ContentType, false);
                // Attempt to read the value of the key
                value = (key == null) ? null : key.GetValue("Extension", null);
                // Build/create the file extension string
                extension = (value == null) ? string.Empty : value.ToString();

                // Create a new Content-Disposition header
                var cd = new System.Net.Mime.ContentDisposition
                {
                    // Assemble the file name + extension
                    FileName = $"img-{stringId}{extension}",
                    // Force the media item to be saved (not viewed)
                    Inline = false
                };
                // Add the header to the response
                Response.AppendHeader("Content-Disposition", cd.ToString());

                return File(o.Content, o.ContentType);
            }
        }

    }
}
